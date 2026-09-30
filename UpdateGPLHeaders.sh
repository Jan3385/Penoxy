# Penoxy
#
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

TITLE="/** Penoxy"
IFS='' read -r -d '' BODYTEXT <<"EOF"

 *
 * Copyright (C) 2026 Penoxy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 *
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
EOF

HEADER=$TITLE$BODYTEXT

echo "Using title: $TITLE"

if [[ ! -e src/ ]];
then
	echo "src/ not found, exiting..."
	exit -1
fi

if [[ ! -e include/ ]];
then
	echo "include/ not found, exiting..."
	exit -1
fi

echo "All files (regardless of extension) located in src/ and include/ will be modified!"

while true
do
	read -r -p 'Do you want to continue? [Y/n]: ' choice
	case "$choice" in
		n|N)
			echo ""
			echo "Exiting."
			exit 0
			;;
		""|y|Y)
			break
			;;
		*)
			echo "Sorry, response '$choice' not understood."
			;;
	esac
done

FILES=$(find src/ include/ -type f)

SKIPPED_COUNT=0

# add the header if not already present (checks for $TITLE anywhere in the file)
for file in $FILES; do
	if grep -Fxq "$TITLE" $file;
	then # found
		SKIPPED_COUNT=$((SKIPPED_COUNT + 1))
		continue
	else # not found
		echo "${HEADER}" | cat - $file > /tmp/out && mv /tmp/out "$file"
		echo "Updated $file"
	fi
done

echo ""
echo "Skipped $SKIPPED_COUNT files"

exit 0

