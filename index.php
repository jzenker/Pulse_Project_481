<!--  Josephine Zenker modified from Lee Johnson's web tech class and David Gowans 
      Script for formatting inserted data into the right columns. 

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
        error_reporting(E_ALL);
      //  ini_set('display_errors', 1);
      //  include("dbinfo.inc.php");
        $con = mysqli_connect("avl.cs.unca.edu", 'jzenker', 'sql4you', 'jzenkerDBTest')
         or die("Unable to select database");

        $query = "SELECT * FROM heart_rate";

        $result = mysqli_query($con, $query);

        mysqli_close($con);

        echo "<br><b><center>DATA</center></b><br>";
        ?>
        <table border="0" cellspacing="2" cellpadding="2">
            <tr> 
                <th>User id</th>
                <th>Time</th>
                <th>HR Value</th>
                <th>Color</th>
         
            </tr>

            <?php
            while ($row = mysqli_fetch_array($result)) {

                $user_id = $row['user_id'];
                $time_stamp = $row['time_stamp'];
                $heart_rate_value = $row['heart_rate_value'];
                $light_color = $row['light_color'];
               
                ?>

                <tr> 
                    <td><?php echo "$user_id"; ?></td>
                    <td><?php echo "$time_stamp"; ?></td>
                    <td><?php echo "$heart_rate_value"; ?></td>
                    <td><?php echo "$light_color"; ?></td>
                </tr>

    <?php
}
echo "</table>";
?>
		<br><br>
 		<center><a href="/~jzenker/jzenker_481B/">Home</a><br></center>
         <hr>
    </body>
</html>
