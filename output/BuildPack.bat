echo BuildPack...
cmd /c makecab -f packet.ddf
move disk1\*.cab .\
rmdir disk1
del setup.inf
del setup.rpt