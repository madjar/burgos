#!/bin/bash
qmake -project -o ts.pro
lupdate ts.pro
rm ts.pro
