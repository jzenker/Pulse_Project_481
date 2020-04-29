<!--  Josephine Zenker modified from Lee Johnson's web tech class and David Gowans 
        Script to insert data into proper rows of table. 
-->
<!DOCTYPE html>
<html>
    <head>
        <title>Pulse Data</title>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <link rel="stylesheet" href="../css/newcss.css">
    </head>
    <body>
<?php

//Use $GET,POST or REQUEST 

$con = mysqli_connect("avl.cs.unca.edu",'jzenker', 'sql4you', 'jzenkerDBTest')
        or die("Unable to select database");


//getting from url, pull off url or form and put them into $ variables 
$user = mysqli_real_escape_string($con, $_REQUEST['user_id']);
$hr = mysqli_real_escape_string($con, $_REQUEST['hr_value']);
$color = mysqli_real_escape_string($con, $_REQUEST['color']);



//Prints to the Arduino NodeMCU console when data is inserting. 
//NOW() gets the time from the database. 
$query = "INSERT INTO heart_rate (user_id, time_stamp, heart_rate_value, light_color) VALUES ('$user', NOW(), '$hr', '$color')";
mysqli_query($con, $query);
if (mysqli_errno($con) != 0) {
    echo mysqli_errno($con) . ": " . mysqli_error($con) . "\n";
} else {
    echo "SQL is...<br>";
    echo $query;
    echo "<br> Rows affected: "; 
    echo mysqli_affected_rows($con);
}

mysqli_close($con);
?> 
    </body>
</html>