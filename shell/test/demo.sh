pwd
echo "Process ID: $$"
echo "File Name: $0"
echo "First Parameter : $1"
echo "Second Parameter : $2"
echo "All parameters 1: $@"
echo "All parameters 2: $*"
echo "bash parameters num: $#"


logfile=results/log/dataNumtest_dist_test.log
conf=config/test_distence.conf

echo $conf
echo $logfile
echo "\n==============================\n" >> $logfile

./bin_Ubuntu_20.04_x86_64_release/test_distence --flagfile=$conf >> $logfile &

echo "last command PID: $!"  #最后执行的后台命令的PID
echo "PID: $!" >> $logfile

tail -f $logfile