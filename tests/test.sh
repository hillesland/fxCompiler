#!/bin/bash

echo This one should fail due to undefined View and Projection.
../bin/Release/fxCompiler dummy.fx

echo Fixing include, should compile ok
../bin/Release/fxCompiler /Iinclude dummy.fx
cp dummy.fxo dummy1.fxo

echo Compile as child effect
../bin/Release/fxCompiler /Iinclude /Gch dummy.fx
cp dummy.fxo dummy2.fxo

echo Compile with debug info
../bin/Release/fxCompiler /Iinclude /Gch /Zi dummy.fx
cp dummy.fxo dummy3.fxo

echo Compile with debug info and optimizations off.
../bin/Release/fxCompiler /Iinclude /Gch /Zi /Od dummy.fx
cp dummy.fxo dummy4.fxo

