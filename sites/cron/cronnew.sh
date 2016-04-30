cur_date=`date +%m-%d-%y%n`
filename=cronlist-$cur_date.txt
touch /tmp/shellcommands/$filename
drush @sites cron -y 2>>/tmp/shellcommands/$filename