#!/bin/bash

MONERO_BRANCH="release-v0.14"

# Check for presence of monero code
if [ ! -d "monero" ] || [ ! -e "monero/.git" ]; then

    # Check out monero code
    echo "Monero tree not found in $PWD - cloning from github..."
    git clone -b $MONERO_BRANCH --recursive https://github.com/monero-project/monero
    pushd monero > /dev/null 2>&1
    git branch --set-upstream-to=origin/$MONERO_BRANCH $MONERO_BRANCH
    popd > /dev/null 2>&1
fi

# Reset monero code to HEAD
pushd monero > /dev/null 2>&1
git checkout -b $MONERO_BRANCH
git reset HEAD --hard
git pull -t
popd > /dev/null 2>&1

# Apply patches / whole files to the monero codebase
pushd patches > /dev/null 2>&1
echo "Applying patches to Monero codebase:"
find * -type f | while read line ; do
    echo -n -e "\t"
    if [[ $line =~ ".git/" ]]; then
        continue
    elif [[ $line =~ "^README.md$" ]]; then
        continue
    fi
    if [[ ${line: -6} == ".patch" ]]; then
        patchfile=$line
        filename=${patchfile//\.patch/}
        dstfilename="`dirname $PWD`/monero/$filename"
        #echo "Applying patch file $patchfile for target $dstfilename ...";
        patch -p0 $dstfilename < $patchfile
    else
        dstfilename="../monero/$line"
	foldername=${line%/*}
        echo "Copying file $line to $dstfilename ...";
	if [[ ! -d "../monero/$foldername" ]]; then
	    mkdir -p "../monero/$foldername"
	fi
        cp $line $dstfilename
    fi
done
popd > /dev/null 2>&1

# Write out the Haven version information
HAVENVER=`git rev-parse --short HEAD`
sed -i -e "s/@HAVENTAG@/$HAVENVER/g" monero/src/version.cpp.in

export USE_SINGLE_BUILDDIR=1

echo "Compiling patched monero code..."
pushd monero > /dev/null 2>&1
make $@

popd > /dev/null 2>&1
echo "Done."
