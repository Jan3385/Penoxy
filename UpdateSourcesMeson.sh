# Copyright (C) 2026 Penoxy
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

#!/usr/bin/env bash

# global vars
WORKING_FILE="meson.build"

if [[ ! -e src/ ]];
then
	echo "src/ not found, exiting..."
	exit -1
fi

# get sources and turn it into a bash array
SOURCES=$(find src -type f -name "*.cpp")
SOURCES_ARRAY=(${SOURCES//;/ })

echo "Found ${#SOURCES_ARRAY[@]} source files in src/"

# add single quotes for each file
for i in "${SOURCES_ARRAY[@]}";
do
	i="'${i}'"
done

# find & replace it in $WORKING_FILE
if [[ ! -e $WORKING_FILE ]];
then
	echo "$WORKING_FILE not found, exiting..."
	exit -1
fi

# remove current sources
SOURCES_LINE_NUMBER=$(awk '/\bsources = files\(\b/{ print NR; exit }' $WORKING_FILE)

FOUND_SOURCES=false
FOUND_END_SOURCES=false

LINE_NUMBER=1
LINES_TO_BE_DELETED=()

# first pass: get source files' line numbers for deletion
while read -r line
do
	# checking for source files beginning
	if [[ $line == "sources = files("* ]];
	then
		FOUND_SOURCES=true
		echo "Found beginning of meson.build files at line $LINE_NUMBER"
		continue
	fi

	# checking for end ')'
	if [ "$FOUND_SOURCES" == true ] && [ "$FOUND_END_SOURCES" == false ] && [ $line == ")" ];
	then
		FOUND_END_SOURCES=true
		FOUND_SOURCES=false
		echo "Found end of meson.build files at line $LINE_NUMBER"
	fi


	LINE_NUMBER=$((LINE_NUMBER+1))
	# source files in "sources = files (...)"
	if [ "$FOUND_SOURCES" == true ] && [ "$FOUND_END_SOURCES" == false ];
	then
		LINES_TO_BE_DELETED+=( $LINE_NUMBER )
	fi
done < $WORKING_FILE

# delete the actual source files
for (( idx=${#LINES_TO_BE_DELETED[@]} ; idx>0 ; idx-- )); do
	sed -i ${LINES_TO_BE_DELETED[idx-1]}'d' $WORKING_FILE
done

# second pass: insert new source files
SOURCES_STRING=""
SOURCE_INDEX=0

# build string for sed command
for source in ${SOURCES_ARRAY[@]}; do
	if [[ $SOURCE_INDEX -ne $((${#SOURCES_ARRAY[@]} - 1)) ]]; then
		SOURCES_STRING+="  $source\n"
	else
		SOURCES_STRING+="  $source"
	fi

	SOURCE_INDEX=$((SOURCE_INDEX + 1))
done

# add the new source files
sed -i "s|\bsources = files(|sources = files(\n$SOURCES_STRING|" $WORKING_FILE

echo "Successfully replaced ${#LINES_TO_BE_DELETED[@]} old source files!"

exit 0
