<!--  Josephine Zenker modified from Lee Johnson's web tech class and David Gowans 
Shell of future work for the project. Currently not functional. 
-->

<!DOCTYPE html>
<html>
    <head>
        <title>Delete ID </title>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <link rel="stylesheet" href="../css/newcss.css">
    </head>
    <body>
<?php

include("dbinfo.inc.php");

$con = mysqli_connect("avl.cs.unca.edu",'jzenker','sql4you', 'jzenkerDBTest')
        or die("Unable to select database");

$id = mysqli_real_escape_string($con, $_REQUEST['user_id']);

$query = "delete from  heart_rate where id='$id' ";

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



