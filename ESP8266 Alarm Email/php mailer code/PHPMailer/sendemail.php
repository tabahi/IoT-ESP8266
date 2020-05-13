<?php

require 'PHPMailerAutoload.php';


if( isset($_GET["toemail"]) && isset($_GET["host"]) && isset($_GET["port"]) && isset($_GET["user"]) && isset($_GET["pass"]) && isset($_GET["from"]) && isset($_GET["replyto"])&& isset($_GET["subject"])&& isset($_GET["body"]))
{
	$hostname = $_GET["host"];
	$port = intval ($_GET["port"]);
	$username_id = $_GET["user"];
	$pass = $_GET["pass"];
	$sendfrom = $_GET["from"];
	$replyto = $_GET["replyto"];
	$subject = $_GET["subject"];
	$body = $_GET["body"];

echo "Trying to email<br>";

	//ob_start();
	$msgtosend = "<html>
					<body>
						<p>".$body."
						</p>
					</body>
				</html>";
	
	$mail=new PHPMailer();
	$mail->CharSet = 'UTF-8';


	$mail->IsSMTP();
	$mail->Host       = $hostname;
	echo "<br>".$hostname."<br>";

	if($_GET["tls"]=='1'){$mail->SMTPSecure = 'tls';
	echo "<br>TLS Enabled<br>";
	
	}
	
	$mail->Port       = $port;
	$mail->SMTPDebug  = 2;
	$mail->SMTPAuth   = true;
	
	
	echo "<br>".$port."<br>";

	$mail->Username   = $username_id;
	$mail->Password   = $pass;
	
	echo "<br>".$Username."<br>";

	$mail->SetFrom($sendfrom, 'Operator');
	$mail->AddReplyTo($replyto,'no-reply');
	$mail->Subject    = $subject;
	$mail->MsgHTML($msgtosend);
	
	echo "<br>".$sendfrom."<br>";
	echo "<br>".$replyto."<br>";
	echo "<br>".$subject."<br>";
	echo "<br>".$msgtosend."<br>";
	echo "<br>".$_GET["toemail"]."<br>";

	$mail->AddAddress($_GET["toemail"], "");
	$mail->send();
	
	echo "<br><br>Done!";
				
}
else
{
	echo "<br><br>Wrong command<br><br>";
}