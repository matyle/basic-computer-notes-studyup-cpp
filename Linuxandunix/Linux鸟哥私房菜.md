
This  flag  provides  the  same  behavior  as  MAP_SHARED  except  that
MAP_SHARED mappings ignore unknown flags in flags.  By  contrast,  when
creating  a  mapping using MAP_SHARED_VALIDATE, the kernel verifies all
passed flags are known and fails the mapping with the error  EOPNOTSUPP
for  unknown  flags.   This mapping type is also required to be able to
use some mapping flags (e.g., MAP_SYNC).