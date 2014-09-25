// class template regex -*- C++ -*-

// Copyright (C) 2013-2014 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/**
 *  @file bits/regex_automaton.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{regex}
 */

// This macro defines the maximal state number a NFA can have.
#ifndef _GLIBCXX_REGEX_STATE_LIMIT
#define _GLIBCXX_REGEX_STATE_LIMIT 100000
#endif

namespace std _GLIBCXX_VISIBILITY(default)
{
namespace __detail
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  /**
   *  @defgroup regex-detail Base and Implementation Classes
   *  @ingroup regex
   *  @{
   */

  typedef long _StateIdT;
  static const _StateIdT _S_invalid_state_id  = -1;

  template<typename _CharT>
    using _Matcher = std::function<bool (_CharT)>;

  /// Operation codes that define the type of transitions within the base NFA
  /// that represents the regular expression.
  enum _Opcode : int
  {
      _S_opcode_unknown,
      _S_opcode_alternative,
      _S_opcode_repeat,
      _S_opcode_backref,
      _S_opcode_line_begin_assertion,
      _S_opcode_line_end_assertion,
      _S_opcode_word_boundary,
      _S_opcode_subexpr_lookahead,
      _S_opcode_subexpr_begin,
      _S_opcode_subexpr_end,
      _S_opcode_dummy,
      _S_opcode_match,
      _S_opcode_accept,
  };

  struct _State_base
  {
    _Opcode      _M_opcode;           // type of outgoing transition
    _StateIdT    _M_next;             // outgoing transition
    union // Since they are mutually exclusive.
    {
      size_t _M_subexpr;        // for _S_opcode_subexpr_*
      size_t _M_backref_index;  // for _S_opcode_backref
      struct
      {
	// for _S_opcode_alternative, _S_opcode_repeat and
	// _S_opcode_subexpr_lookahead
	_StateIdT  _M_alt;
	// for _S_opcode_word_boundary or _S_opcode_subexpr_lookahead or
	// quantifiers (ungreedy if set true)
	bool       _M_neg;
      };
    };

    explicit _State_base(_Opcode __opcode)
    : _M_opcode(__opcode), _M_next(_S_invalid_state_id)
    { }

  protected:
    ~_State_base() = default;

  public:
#ifdef _GLIBCXX_DEBUG
    std::ostream&
    _M_print(std::ostream& ostr) const;

    // Prints graphviz dot commands for state.
    std::ostream&
    _M_dot(std::ostream& __ostr, _StateIdT __id) const;
#endif
  };

  template<typename _TraitsT>
    struct _State : _State_base
    {
      typedef _Matcher<typename _TraitsT::char_type> _MatcherT;

      _MatcherT      _M_matches;        // for _S_opcode_match

      explicit _State(_Opcode __opcode) : _State_base(__opcode) { }
    };

  struct _NFA_base
  {
    typedef size_t                              _SizeT;
    typedef regex_constants::syntax_option_type _FlagT;

    explicit
    _NFA_base(_FlagT __f)
    : _M_flags(__f), _M_start_state(0), _M_subexpr_count(0),
    _M_has_backref(false)
    { }

    _NFA_base(_NFA_base&&) = default;

  protected:
    ~_NFA_base() = default;

  public:
    _FlagT
    _M_options() const
    { return _M_flags; }

    _StateIdT
    _M_start() const
    { return _M_start_state; }

    _SizeT
    _M_sub_count() const
    { return _M_subexpr_count; }

    std::vector<size_t>       _M_paren_stack;
    _FlagT                    _M_flags;
    _StateIdT                 _M_start_state;
    _SizeT                    _M_subexpr_count;
    bool                      _M_has_backref;
  };

  template<typename _TraitsT>
    struct _NFA
    : _NFA_base, std::vector<_State<_TraitsT>>
    {
      typedef _State<_TraitsT>				_StateT;
      typedef _Matcher<typename _TraitsT::char_type>	_MatcherT;

      using _NFA_base::_NFA_base;

      // for performance reasons _NFA objects should only be moved not copied
      _NFA(const _NFA&) = delete;
      _NFA(_NFA&&) = default;

      _StateIdT
      _M_insert_accept()
      {
	auto __ret = _M_insert_state(_StateT(_S_opcode_accept));
	return __ret;
      }

      _StateIdT
      _M_insert_alt(_StateIdT __next, _StateIdT __alt, bool __neg)
      {
	_StateT __tmp(_S_opcode_alternative);
	// It labels every quantifier to make greedy comparison easier in BFS
	// approach.
	__tmp._M_next = __next;
	__tmp._M_alt = __alt;
	return _M_insert_state(std::move(__tmp));
      }

      _StateIdT
      _M_insert_repeat(_StateIdT __next, _StateIdT __alt, bool __neg)
      {
	_StateT __tmp(_S_opcode_repeat);
	// It labels every quantifier to make greedy comparison easier in BFS
	// approach.
	__tmp._M_next = __next;
	__tmp._M_alt = __alt;
	__tmp._M_neg = __neg;
	return _M_insert_state(std::move(__tmp));
      }

      _StateIdT
      _M_insert_matcher(_MatcherT __m)
      {
	_StateT __tmp(_S_opcode_match);
	__tmp._M_matches = std::move(__m);
	return _M_insert_state(std::move(__tmp));
      }

      _StateIdT
      _M_insert_subexpr_begin()
      {
	auto __id = this->_M_subexpr_count++;
	this->_M_paren_stack.push_back(__id);
	_StateT __tmp(_S_opcode_subexpr_begin);
	__tmp._M_subexpr = __id;
	return _M_insert_state(std::move(__tmp));
      }

      _StateIdT
      _M_insert_subexpr_end()
      {
	_StateT __tmp(_S_opcode_subexpr_end);
	__tmp._M_subexpr = this->_M_paren_stack.back();
	this->_M_paren_stack.pop_back();
	return _M_insert_state(std::move(__tmp));
      }

      _StateIdT
      _M_insert_backref(size_t __index);

      _StateIdT
      _M_insert_line_begin()
      { return _M_insert_state(_StateT(_S_opcode_line_begin_assertion)); }

      _StateIdT
      _M_insert_line_end()
      { return _M_insert_state(_StateT(_S_opcode_line_end_assertion)); }

      _StateIdT
      _M_insert_word_bound(bool __neg)
      {
	_StateT __tmp(_S_opcode_word_boundary);
	__tmp._M_neg = __neg;
	return _M_insert_state(std::move(__tmp));
      }

      _StateIdT
      _M_insert_lookahead(_StateIdT __alt, bool __neg)
      {
	_StateT __tmp(_S_opcode_subexpr_lookahead);
	__tmp._M_alt = __alt;
	__tmp._M_neg = __neg;
	return _M_insert_state(std::move(__tmp));
      }

      _StateIdT
      _M_insert_dummy()
      { return _M_insert_state(_StateT(_S_opcode_dummy)); }

      _StateIdT
      _M_insert_state(_StateT __s)
      {
	this->push_back(std::move(__s));
	if (this->size() > _GLIBCXX_REGEX_STATE_LIMIT)
	  __throw_regex_error(regex_constants::error_space);
	return this->size()-1;
      }

      // Eliminate dummy node in this NFA to make it compact.
      void
      _M_eliminate_dummy();

#ifdef _GLIBCXX_DEBUG
      std::ostream&
      _M_dot(std::ostream& __ostr) const;
#endif
    };

  /// Describes a sequence of one or more %_State, its current start
  /// and end(s).  This structure contains fragments of an NFA during
  /// construction.
  template<typename _TraitsT>
    class _StateSeq
    {
    public:
      typedef _NFA<_TraitsT> _RegexT;

    public:
      _StateSeq(_RegexT& __nfa, _StateIdT __s)
      : _M_nfa(__nfa), _M_start(__s), _M_end(__s)
      { }

      _StateSeq(_RegexT& __nfa, _StateIdT __s, _StateIdT __end)
      : _M_nfa(__nfa), _M_start(__s), _M_end(__end)
      { }

      // Append a state on *this and change *this to the new sequence.
      void
      _M_append(_StateIdT __id)
      {
	_M_nfa[_M_end]._M_next = __id;
	_M_end = __id;
      }

      // Append a sequence on *this and change *this to the new sequence.
      void
      _M_append(const _StateSeq& __s)
      {
	_M_nfa[_M_end]._M_next = __s._M_start;
	_M_end = __s._M_end;
      }

      // Clones an entire sequence.
      _StateSeq
      _M_clone();

    public:
      _RegexT&  _M_nfa;
      _StateIdT _M_start;
      _StateIdT _M_end;
    };

 //@} regex-detail
_GLIBCXX_END_NAMESPACE_VERSION
} // namespace __detail
} // namespace std

#include <bits/regex_automaton.tcc>
