#!/bin/bash

# Exclui arquivos de backup
rm *~ .*~ 2> '/dev/null'
rm include/*~ include/.*~ 2> '/dev/null'
rm libraries/*~ libraries/.*~ 2> '/dev/null'
