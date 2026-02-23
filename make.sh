#!/usr/bin/bash

gcc -o tester -g src/lexer.c src/string_interner.c src/tester.c src/tokenizer.c
