# Olivier Stasse
# JRL/CNRS Copyright
# Gnuplot file to display analysis of
# walkGenJRL computation.
#
plot "RebuildZMP.dat" u 5 w l t "LF-A-X", \
  "RebuildZMP.dat" u 8 w l t "RF-A-X", \
  "RebuildZMP.dat" u 1 w l t "ZMP-A-X", \
  "jl_pgleftfootref.dat" u ($0-33720):5 w l t "LF-Ref-X", \
  "jl_pgrightfootref.dat"  u ($0-33720):5 w l t "RF-Ref-X"
