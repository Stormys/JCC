# JCC
This is my attempt to create a complete compiler for the c programming language working on a unix machine.

## Functionality
PreProccessor:
* pragma are ignored for now
* if/elif eval isnt done
* might mess up on if, endif and else tree
* probably issues on macro expansions? (mostly function ones)
* no stringify
* or concatenation
* using GCC standard library wont work due to not having builtins/predefined macros yet

Scanner (Tokenization):
* numbers in float/decimal format
* \ at end of line may or not work as expected.

## License
JCC is released under [MIT License](LICENSE).
