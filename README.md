# Tetrashell
###### Created by Nicole Arwood and Suzanna Moran
* Implemented as described in the write-up:
  * Check
  * Recover
  * Modify
  * Rank
  * Exit
  * Quick rank
  * Switch current quicksave
  * Help
  * Save info
  * Undo

* Improved prompt:
  * Includes username, abbreviated filepath, current score, and current line

* Pretty recover:
  * Implemented as described in the write-up, with the added bonus that the color of the text switches each line, looping through six color values to form a rainbow as you scroll down.

* The world's most beautiful title screen:
  * "Tetrashell" appears in the flower-power ASCII art font from Patrick Gillespie's ASCII text generator. We pasted the text into a text file (flower.txt) to preserve the formatting, then read the file character by character inside tetrashell.c to print to stdout. The flowers were printed in yellow, and the text was printed in blue.

