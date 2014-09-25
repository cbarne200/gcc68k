-- CD5014X.ADA

--                             Grant of Unlimited Rights
--
--     Under contracts F33600-87-D-0337, F33600-84-D-0280, MDA903-79-C-0687,
--     F08630-91-C-0015, and DCA100-97-D-0025, the U.S. Government obtained 
--     unlimited rights in the software and documentation contained herein.
--     Unlimited rights are defined in DFAR 252.227-7013(a)(19).  By making 
--     this public release, the Government intends to confer upon all 
--     recipients unlimited rights  equal to those held by the Government.  
--     These rights include rights to use, duplicate, release or disclose the 
--     released technical data and computer software in whole or in part, in 
--     any manner and for any purpose whatsoever, and to have or permit others 
--     to do so.
--
--                                    DISCLAIMER
--
--     ALL MATERIALS OR INFORMATION HEREIN RELEASED, MADE AVAILABLE OR
--     DISCLOSED ARE AS IS.  THE GOVERNMENT MAKES NO EXPRESS OR IMPLIED 
--     WARRANTY AS TO ANY MATTER WHATSOEVER, INCLUDING THE CONDITIONS OF THE
--     SOFTWARE, DOCUMENTATION OR OTHER INFORMATION RELEASED, MADE AVAILABLE 
--     OR DISCLOSED, OR THE OWNERSHIP, MERCHANTABILITY, OR FITNESS FOR A
--     PARTICULAR PURPOSE OF SAID MATERIAL.
--*
-- OBJECTIVE:
--     CHECK THAT AN ADDRESS CLAUSE CAN BE GIVEN IN THE PRIVATE PART
--     OF A GENERIC PACKAGE SPECIFICATION FOR A VARIABLE OF A FORMAL
--     ARRAY TYPE, WHERE THE VARIABLE IS DECLARED IN THE VISIBLE PART
--     OF THE SPECIFICATION.

-- HISTORY:
--     BCB 10/08/87  CREATED ORIGINAL TEST.
--     PWB 05/11/89  CHANGED EXTENSION FROM '.DEP' TO '.ADA'.

WITH SYSTEM; USE SYSTEM;
WITH SPPRT13; USE SPPRT13;
WITH REPORT; USE REPORT;
WITH TEXT_IO; USE TEXT_IO;

PROCEDURE CD5014X IS

BEGIN

     TEST ("CD5014X", " AN ADDRESS CLAUSE CAN BE GIVEN " &
                      "IN THE PRIVATE PART OF A GENERIC PACKAGE " &
                      "SPECIFICATION FOR A VARIABLE OF A FORMAL " &
                      "ARRAY TYPE, WHERE THE VARIABLE IS DECLARED " &
                      "IN THE VISIBLE PART OF THE SPECIFICATION");

     DECLARE

          TYPE COLOR IS (RED,BLUE,GREEN);
          TYPE COLOR_TABLE IS ARRAY (COLOR) OF INTEGER;

     GENERIC
          TYPE INDEX IS (<>);
          TYPE FORM_ARRAY_TYPE IS ARRAY (INDEX) OF INTEGER;
     PACKAGE PKG IS
          FORM_ARRAY_OBJ1 : FORM_ARRAY_TYPE := (1,2,3);
     PRIVATE
          FOR FORM_ARRAY_OBJ1 USE AT VARIABLE_ADDRESS;
     END PKG;

     PACKAGE BODY PKG IS
     BEGIN

          IF EQUAL(3,3) THEN
               FORM_ARRAY_OBJ1 := (10,20,30);
          END IF;

          IF FORM_ARRAY_OBJ1 /= (10,20,30) THEN
               FAILED ("INCORRECT VALUE FOR FORMAL ARRAY VARIABLE");
          END IF;

          IF FORM_ARRAY_OBJ1'ADDRESS /= VARIABLE_ADDRESS THEN
               FAILED ("INCORRECT ADDRESS FOR FORMAL ARRAY " &
                       "VARIABLE");
          END IF;
     END PKG;

     PACKAGE PACK IS NEW PKG(INDEX => COLOR,
                             FORM_ARRAY_TYPE => COLOR_TABLE);

     BEGIN
          NULL;
     END;

     RESULT;
END CD5014X;
