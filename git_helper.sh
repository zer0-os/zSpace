max_size=1

modified_with_info=$(git status | grep "modified:" | awk '{print $2}')
untracked_with_info=$(git status --untracked-files=all | sed -n '/Untracked files:/,/^$/p' | grep -v "use \"git add <file>\|Untracked files" | awk '{print $1}')
#echo "$untracked_with_info"

check_if_file_is_big() {
    size=$(ls -lh $1 | awk '{print $5}')
    round_num=0
    echo $size | grep -qi "m"
    [[ $? == 0 ]] && size_pure=$(echo $size | tr -d "[a-z][A-Z)") \
    && round_num=$(echo $size_pure| awk -F "." '{print $1}')

    if [ $round_num -ge "$max_size" ] ; then
            echo $1 $size
        fi
}

IFS=$'\n';
filter_big_files() {
    for i in $1; do
          check_if_file_is_big $i
     done
}

echo "*** Getting modified files greater than $max_size MB .."
filter_big_files "$modified_with_info"
echo
echo
echo "*** Getting untracked files greater than $max_size MB .."
filter_big_files "$untracked_with_info"

#files_in_sub=`ls toremove/**/*`
#echo $files_in_sub
