<?php
$server_name="localhost";
$username="root";
$password="";
$db_name="crud";

$conn=mysqli_connect($server_name,$username,$password,$db_name);
if($conn==TRUE){
 echo "successful";
}
else{
  echo "unsuccessful";
}



?>