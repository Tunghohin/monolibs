file(REMOVE_RECURSE
  "libmonolibs.pdb"
  "libmonolibs.so"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/monolibs.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
