#!/bin/bash
qmake -project -o ts.pro
lrelease ts.pro
rm ts.pro
