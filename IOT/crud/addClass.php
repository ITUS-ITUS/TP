<?php
 include 'header.php'; 
include 'config.php';
//echo "PHP self page url is ".$_SERVER["PHP_SELF"];
if($_SERVER["REQUEST_METHOD"] == "POST")
{

$cname="";
$cname=$_POST['cname'];

$sql_insert="INSERT INTO studentclass(`cid`, `cname`)
 VALUES (NULL,'$cname')";

if(mysqli_query($conn,$sql_insert))
{
   echo "Class Inserted Successfully";
  }
else{
echo "Error";
}
}

?>

<html>
<head>
<title>REGISTRATION FORM</title>
<body>

<form name="registration" method="post" 
action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]);?>">

<h1>Add Class Here</h1>
ClassName:<input type="text" name="cname" value=""></br>
<br>
<input type="submit" name="submit" value="submit" >
</form>

</body>
</head>
</html>