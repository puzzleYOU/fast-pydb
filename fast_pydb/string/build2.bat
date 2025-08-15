cython __init__.pyx
set LIB=%LIB%;"C:\Program Files\Python313\libs"
cl __init__.c clean_string.c /I "C:\Program Files\Python313\Include" /LD
move /Y __init__.dll __init__.pyd