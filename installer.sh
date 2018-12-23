#echo "pwd=" $PWD
#echo "to dir = " $TODIR
#echo "packdir = " $PACKDIR
cd $TODIR
until [ -z "$1" ]
do 
  ln -s $PACKDIR/"$1"
  shift
done
cd $PACKDIR/..