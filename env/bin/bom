#!/bin/bash

if [ "" = "$1" ]; then
	echo "Convert file from GB18030 to UTF-8 with BOM"
	echo "Usage: bom <filename>"
	exit -1
fi

echo Convert $1 from GB18030 to UTF-8 with BOM...
temp_file=bomtemptemp_$1
echo -ne '\xEF\xBB\xBF'>$temp_file
iconv -f GB18030 -t UTF-8 $1>>$temp_file
mv -f $temp_file $1
