<?php /*?><?php
$name = $_GET['name'];

$connection = mysqli_connect("localhost:9080", "root", "", "iot"); // Establishing Connection with Server..
$query = mysqli_query($connection, "SELECT status from iot WHERE name = '" . $name ."'"); //Update Query
if(! $query) {
    die("SQL Error: " . mysqli_error($connection));
} else {
    $row = $query->fetch_array(MYSQL_BOTH);
    echo $row['status'];
}
?>
<?php */?><?php
$servername = "127.0.0.1";
$username = "root";
$password = "";
$dbname = "iot";

$name = $_GET['Name'];

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);

// Check connection
if ($conn->connect_error) {
  die("Connection failed: " . $conn->connect_error);
}

$sql = "SELECT * from iot WHERE name = '" . $name . "'";

$result = $conn->query($sql);

if ($result->num_rows > 0) {
  // output data of each row
  while($row = $result->fetch_assoc()) {
    echo $row['status'];
  }
} else {
  echo "0 results";
}
$conn->close();

?>