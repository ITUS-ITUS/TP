<?php

$stu_id = $_GET['sid'];

include 'config.php';

$sql = "DELETE FROM student WHERE sid = {$stu_id}";
$result = mysqli_query($conn, $sql) or die("Query Unsuccessful.");

header("Location: http://localhost/php-exp/mobile_store/index.php");


mysqli_close($conn);

?>
