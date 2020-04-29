<!--  Josephine Zenker modified from Lee Johnson's web tech class and David Gowans
        Shell of future work for the project. Currently not functional. Similar to index.php script. 
 -->
<!DOCTYPE html>
<html>
    <head>
        <title>Update HR</title>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <link rel="stylesheet" href="../css/newcss.css">
    </head>
    <body>
        <?php
        error_reporting(E_ALL);
        ini_set('display_errors', 1);
        include("dbinfo.inc.php");

        $idin = $_REQUEST['user_id'];

        $con = mysqli_connect("avl.cs.unca.edu",'jzenker', 'sql4you', 'jzenkerDBTest')
                or die("Unable to select database");

        $query = "SELECT * FROM heart_rate WHERE id='$idin'";
        $result = mysqli_query($con, $query);

//$num = mysqli_num_rows($result);
//echo $num;
        mysqli_close($con);

        echo "<b><center>Update Contacts Database</center></b><br>";


        while ($row = mysqli_fetch_array($result)) {

            $id = $row['user_id'];
            $time_stamp = $row['time_stamp'];
            $heart_rate_value = $row['heart_rate_value'];
            $light_color = $row['light_color'];
        }

        echo "Selected ID: $id";

        ?>
        <form action="update3.php"> 
            <input type="hidden" name="id" 
                   value=<?php echo $id; ?> >
            Time: <input type="text" name="time_stamp" 
                               value=<?php echo $time_stamp; ?> > <br>
            HR Value: <input type="text" name="heart_rate_value" 
                              value=<?php echo $heart_rate_value; ?> > <br>
            Light Color: <input type="text" name="light_color" 
                          value=<?php echo $light_color; ?> > <br>
           
            <input type="Submit" value="Submit">
        </form>
    </body>
</html>


