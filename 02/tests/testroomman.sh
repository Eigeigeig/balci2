#!/bin/bash
ROOMMAN="./roommanager"
rm -f *.dat

okayesno()
{
    echo -n "OK? (j/n)"
    read ok
    if [ "$ok" \= "n" ]; then
        echo "Error in Test $1" >>protocol.txt
    fi
}


check_interactive()
{
	testno=$1
	expect=$2
	cmd=$3
	errmsg=$4
    received=$(${cmd})
	echo "*********************************************"
	echo "** $testno Expected:"
	echo "$expect"
	echo "** $testno Found:"
	echo "$received"
	echo "*********************************************"
	okayesno "$testno $errmsg"
}

check_noninteractive()
{
	testno=$1
	expect=$2
	cmd=$3
	errmsg=$4
	received=`$cmd`
	echo "$received" >__received.txt
	echo "$expect">__expected.txt
	difference=`diff -b __received.txt __expected.txt`
	rm -f __received.txt __expected.txt	
	if [ "$difference" ] ; then
		echo "*********************************************"
		echo "** $testno: Descrepancies:"
		echo "$difference"
		echo "*********************************************"
		okayesno "$testno $errmsg"
	fi
}


check=check_interactive

for i in "$@"
do
case $i in
    -n|--noninteractive)
	check=check_noninteractive
    shift # past argument
    ;;
    -i|--interactive)
	check=check_interactive
    shift # past argument
    ;;
    --default)
    shift # past argument with no value
    ;;
    *)
          # unknown option
    ;;
esac
done


echo "*********************************************"
echo "**               Roommantest               **"
echo "*********************************************"
sleep 1

echo "*********************************************"
echo "**       0    RTFM-Nachricht               **"
echo "*********************************************"
rtfm=$(${ROOMMAN})
if [ -z "$rtfm" ] ; then
	echo "*********************************************"
	echo "** 0 Expected:"
	echo "RTFM message"
	echo "** $testno Found:"
	echo "*NOTHING* -> Fatal"
	echo "*********************************************"
	okayesno "0 Fatal:No_RTFM"
	exit
fi

echo "*********************************************"
echo "**       1    Normal Usage                 **"
echo "*********************************************"

$ROOMMAN -n gebaeude1 129 82
$ROOMMAN -n BCN 421 22
$ROOMMAN -n HOST HOST
$ROOMMAN -n BCN 331 50

testno="1.1"
read -r -d '' expect <<'EOF'
Building                        Name                            Occupied Capacity Reserved
gebaeude1                       129                                    0       82 False
BCN                             421                                    0       22 False
HOST                            HOST                                   0       10 False
BCN                             331                                    0       50 False
EOF
cmd="$ROOMMAN -l"
$check "$testno" "$expect" "$cmd" "-l Error"


testno="1.2"
read -r -d '' expect <<'EOF'
Building                        Name                            Occupied Capacity        Reserved
BCN                             421                                    0       22        False
BCN                             331                                    0       50 False
EOF
cmd="$ROOMMAN -l BCN"
$check "$testno" "$expect" "$cmd" "-l Name Error"


testno="1.3.1"
expect="Updated numbers of persons occupying room 129 in building gebaeude1 to 12"
cmd="$ROOMMAN -o gebaeude1 129 12"
$check "$testno" "$expect" "$cmd" "-f Error"

testno="1.3.2"
expect="Updated room capacity for room 129 in building gebaeude1 to 72"
cmd="$ROOMMAN -c gebaeude1 129 72"
$check "$testno" "$expect" "$cmd" "-f Error"

testno="1.3.3"
read -r -d '' expect <<'EOF'
Building                        Name                            Occupied Capacity        Reserved
gebaeude1                       129                                   12       72        False
EOF
cmd="$ROOMMAN -l gebaeude1"
$check "$testno" "$expect" "$cmd" "-f Error"


testno="1.4"
read -r -d '' expect <<'EOF'
Reserved room 421 in building BCN
EOF
cmd="$ROOMMAN -t BCN 421"
$check "$testno" "$expect" "$cmd" "-s Error"

testno="1.5"
read -r -d '' expect <<'EOF'
Room HOST in building HOST deleted
EOF
cmd="$ROOMMAN -d HOST HOST"
$check "$testno" "$expect" "$cmd" "-d Name Error"


testno="1.6"
read -r -d '' expect <<'EOF'
Building                        Name                            Occupied Capacity Reserved
gebaeude1                       129                                   12       72 False
BCN                             421                                    0       22 True
BCN                             331                                    0       50 False
EOF
cmd="$ROOMMAN -l"
$check "$testno" "$expect" "$cmd" "-l nach löschen -> Error"


echo "*********************************************"
echo "**       2    Falsche Kommandos            **"
echo "*********************************************"

sleep 1

testno="2.1"
expect="$rtfm"
cmd="$ROOMMAN"
$check "$testno" "$expect" "$cmd" "no option -> no error"


testno="2.2"
expect="$rtfm"
cmd="$ROOMMAN -a"
$check "$testno" "$expect" "$cmd" "invalid option -> no error"


testno="2.3"
expect="$rtfm"

cmd="$ROOMMAN -l blah blubb"
$check "$testno" "$expect" "$cmd" "-l too many args -> no error"


testno="2.4"
expect="$rtfm"

cmd="$ROOMMAN -n blah ratz 22 fatz"
$check "$testno" "$expect" "$cmd" "-n too many args -> no error"


testno="2.5"
expect="$rtfm"
cmd="$ROOMMAN -n blah"
$check "$testno" "$expect" "$cmd" "-n too little args -> no error"


testno="2.6"
expect="$rtfm"
cmd="$ROOMMAN -n"
$check "$testno" "$expect" "$cmd" "-n no args -> no error"


testno="2.7"
expect="$ROOMMAN: Not a number: fatz"
cmd="$ROOMMAN -n blah ratz fatz"
$check "$testno" "$expect" "$cmd" "-n no number -> no error"


testno="2.8"
expect="$rtfm"
cmd="$ROOMMAN -c blah ratz 22 fatz"
$check "$testno" "$expect" "$cmd" "-c too many args -> no error"


testno="2.9"
expect="$rtfm"
cmd="$ROOMMAN -c blah ratz"
$check "$testno" "$expect" "$cmd" "-c too little args -> no error"


testno="2.10"
expect="$rtfm"
cmd="$ROOMMAN -c blah"
$check "$testno" "$expect" "$cmd" "-c too little args -> no error"


testno="2.11"
expect="$rtfm"
cmd="$ROOMMAN -c"
$check "$testno" "$expect" "$cmd" "-c no args -> no error"


testno="2.12"
expect="$ROOMMAN: Not a number: fatz"
cmd="$ROOMMAN -c blah ratz fatz"
$check "$testno" "$expect" "$cmd" "-c no number -> no error"


testno="2.13"
expect="$rtfm"
cmd="$ROOMMAN -t blah ratz fatz"
$check "$testno" "$expect" "$cmd" "-s too many args -> no error"


testno="2.14"
expect="$rtfm"
cmd="$ROOMMAN -t blah"
$check "$testno" "$expect" "$cmd" "-s too little args -> no error"


testno="2.15"
expect="$rtfm"
cmd="$ROOMMAN -t"
$check "$testno" "$expect" "$cmd" "-s no args -> no error"


testno="2.16"
expect="$rtfm"
cmd="$ROOMMAN -d blah ratz fatz"
$check "$testno" "$expect" "$cmd" "-d too many args -> no error"

testno="2.17"
expect="$rtfm"
cmd="$ROOMMAN -d blah"
$check "$testno" "$expect" "$cmd" "-s too little args -> no error"

testno="2.18"
expect="$rtfm"
cmd="$ROOMMAN -d"
$check "$testno" "$expect" "$cmd" "-b no args -> no error"


echo "*********************************************"
echo "**      3    Datenbezogene Tests           **"
echo "*********************************************"

sleep 1

testno="3.1"
expect="No rooms matching criteria found"
cmd="$ROOMMAN -l bahnhof"
$check "$testno" "$expect" "$cmd" "-l unknown building -> no error"


testno="3.2.1"
expect="Reserved room 331 in building BCN"
cmd="$ROOMMAN -t BCN 331"
$check "$testno" "$expect" "$cmd" "-t Error"

testno="3.2.2"
expect="Cleared reservation for room 331 in building BCN"
cmd="$ROOMMAN -t BCN 331"
$check "$testno" "$expect" "$cmd" "-t Error"

testno="3.2.3"
expect="Could not reserve non-empty room 129 in building gebaeude1"
cmd="$ROOMMAN -t gebaeude1 129"
$check "$testno" "$expect" "$cmd" "-t Error"

testno="3.2.4"
expect="Room 129 in building gebaeude1 deleted"
cmd="$ROOMMAN -d gebaeude1 129"
$check "$testno" "$expect" "$cmd" "-d Name Error"

testno="3.2.5"
expect="Room 421 in building BCN deleted"
cmd="$ROOMMAN -d BCN 421"
$check "$testno" "$expect" "$cmd" "-d Name Error"

testno="3.2.6"
expect="Room 331 in building BCN deleted"
cmd="$ROOMMAN -d BCN 331"
$check "$testno" "$expect" "$cmd" "-d Name Error"

testno="3.2.7"
expect="No rooms matching criteria found"
cmd="$ROOMMAN -l"
$check "$testno" "$expect" "$cmd" "-l no rooms, but displaying"

testno="3.3.1" 
expect="Done!"
cmd="$ROOMMAN -n gebaeude4 110"
$check "$testno" "$expect" "$cmd" "-n Error"

testno="3.3.2" 
expect="In function roomman_create_room(): Error: the room already exists"
cmd="$ROOMMAN -n gebaeude4 110"
$check "$testno" "$expect" "$cmd" "-n room already exists -> no error"

testno="3.4.1"
expect="Done!"
cmd="$ROOMMAN -n HOST HOST"
$check "$testno" "$expect" "$cmd" "-n doesn't work"

testno="3.4.2"
expect="Done!"
cmd="$ROOMMAN -n BCN 331"
$check "$testno" "$expect" "$cmd" "-n doesn't work"

testno="3.5.1"
expect="In function roomman_create_room(): Error: the room already exists"
cmd="$ROOMMAN -n HOST HOST"
$check "$testno" "$expect" "$cmd" "-n room already exists -> no error"

testno="3.5.2"
expect="In function roomman_create_room(): Error: the room already exists"
cmd="$ROOMMAN -n BCN 331"
$check "$testno" "$expect" "$cmd" "-n room already exists -> no error"

testno="3.6" 
expect="In function roomman_lookup(): Error: Room not found"
cmd="$ROOMMAN -c bahnhof schalter 1"
$check "$testno" "$expect" "$cmd" "-c unknown building -> no error"


testno="3.7 Expected: "
expect="In function roomman_lookup(): Error: Room not found"
cmd="$ROOMMAN -t bahnhof schalter"
$check "$testno" "$expect" "$cmd" "-s unknown room -> no error"

testno="3.8"
expect="In function roomman_lookup(): Error: Room not found"
cmd="$ROOMMAN -d bahnhof schalter"
$check "$testno" "$expect" "$cmd" "-d unknown room -> no error"


echo "*********************************************"
echo "**      4   zu lange Namen                 **"
echo "*********************************************"

sleep 1

testno="4.1"
expect="In function roomman_directory(): Error: the specified name is too long"
cmd="$ROOMMAN -l abcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghi"
$check "$testno" "$expect" "$cmd" "-l long name -> no error"


testno="4.2"
expect="In function roomman_create_room(): Error: the specified name is too long"
cmd="$ROOMMAN -n abcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghi abcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghi"
$check "$testno" "$expect" "$cmd" "-n two long names -> no error"


testno="4.3"
expect="In function roomman_create_room(): Error: the specified name is too long"
cmd="$ROOMMAN -n abcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghi gebaeude4_110"
$check "$testno" "$expect" "$cmd" "-n one long name -> no error"


testno="4.4"
expect="In function roomman_create_room(): Error: the specified name is too long"
cmd="$ROOMMAN -n BCN abcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghi" 
$check "$testno" "$expect" "$cmd" "-n one long name -> no error"


testno="4.5"
expect="In function roomman_lookup(): Error: the specified name is too long"
cmd="$ROOMMAN -c abcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghi abcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghi 234"
$check "$testno" "$expect" "$cmd" "-c two long names -> no error"

testno="4.5"
expect="In function roomman_lookup(): Error: the specified name is too long"
cmd="$ROOMMAN -c abcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghi gebaeude4_110 567"
$check "$testno" "$expect" "$cmd" "-c one long name -> no error"

testno="4.6"
expect="In function roomman_lookup(): Error: the specified name is too long"
cmd="$ROOMMAN -t BCN abcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghi"
$check "$testno" "$expect" "$cmd" "-t one long name -> no error"

testno="4.7"
expect="In function roomman_lookup(): Error: the specified name is too long"
cmd="$ROOMMAN -t abcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghi abcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghi"
$check "$testno" "$expect" "$cmd" "-t two long names -> no error"

testno="4.7"
expect="In function roomman_lookup(): Error: the specified name is too long"
cmd="$ROOMMAN -d abcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghi gebaeude4_110"
$check "$testno" "$expect" "$cmd" "-s one long name -> no error"
