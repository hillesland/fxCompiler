set fxCompiler=..\bin\Release\fxCompiler
@echo This one should fail due to undefined View and Projection.
%fxCompiler% dummy.fx

@echo Fixing include, should compile ok
%fxCompiler% /Iinclude dummy.fx
@copy dummy.fxo dummy1.fxo

@echo Compile as child effect
%fxCompiler% /Iinclude /Gch dummy.fx
@copy dummy.fxo dummy2.fxo

@echo Compile with debug info
%fxCompiler% /Iinclude /Gch /Zi dummy.fx
@copy dummy.fxo dummy3.fxo

@echo Compile with debug info and optimizations off.
%fxCompiler% /Iinclude /Gch /Zi /Od dummy.fx
@copy dummy.fxo dummy4.fxo

