REM IM.convert %1 %2 -compose difference -composite -evaluate Pow .05 %3
IM.convert %1 %2 -compose difference -composite -evaluate Pow 2 -evaluate divide 3 -separate -evaluate-sequence Add -evaluate Pow 0.5 %3
