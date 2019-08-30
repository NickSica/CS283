#!/bin/sh

time -o v1.log for i in {1..10}; do ./$^; done
