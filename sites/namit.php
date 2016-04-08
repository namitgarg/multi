<?php
cron_shell();
function cron_shell(){
	

exec("cron5.sh");
print("done");
}
?>