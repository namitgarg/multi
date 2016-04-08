cur_date=`date +%m-%d-%y%n`
filename=cronlist-$cur_date.txt
touch $filename
drush @sites cron -y 2>>$filename