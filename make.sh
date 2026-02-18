#!/usr/bin/bash

clang -o tester -g src/lexer.c src/string_interner.c src/tester.c src/tokenizer.c
