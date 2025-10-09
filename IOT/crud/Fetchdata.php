<?php
include "config.php";

$query_select="select * from student ";
if($result=mysqli_query($conn,$query_select)){
//echo "Run Successfully";
    while($row=mysqli_fetch_assoc($result)){
      $stud_id=$row['sid'];
      $name=$row['sname'];
      $adress=$row['saddress'];
      echo "<br>". $stud_id ."   ".$name. "   ". $adress ;
    }
}
else{
  echo "Error in Select Statement";
}
?>