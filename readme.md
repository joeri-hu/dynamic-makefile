```makefile
# Dynamic makefile
#
# File:     makefile
# Version:  0.7
#
# Author:   Joeri Kok
# Date:     May 2021
#
# Description:
#   Generates makefile recipes by resolving file-header inclusions recursively.
#
# Known limitations:
#   Basically any project environment that isn't considered trivial. The search
#   mechanisms are implemented quite naively and can be exploited with relative
#   ease.
#
# To-do:
#   - Add support for separation of inc/src directories
#   - Improve search functionality robustness
#   - Write applicable unit-tests
#
```