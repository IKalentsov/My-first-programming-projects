Solution Platforms - x86
VS2019CE

Developer Spice-like Simulator: https://github.com/TopGun-DICD (Dmitry Bulakh)

Technical task:
Реализовать модули решения и записи результатов моделирования.
Метод решения: Метод Гаусса.
Формат записи результатов моделирования: PSF:
HEADER
"PSFversion" "1.00"
"simulator" "KSI"
"runtype" "Transient Analysis"
TYPE
"node" FLOAT DOUBLE PROP(
"key" "node"
)
"branch" FLOAT DOUBLE PROP(
"key" "branch"
)
"sweep" FLOAT DOUBLE
SWEEP
"time" "sweep"
TRACE
"group" GROUP 3
"1" "node"
"0" "node"
"2" "node"
VALUE
"time" 0.000000e+00
"group"
0.000000e+00
0.000000e+00
0.000000e+00
"time" 1.000000e-09
"group"
0.000000e+00
0.000000e+00
0.000000e+00
"time" 2.000000e-09