
<html>
  <head>
  <script type="text/javascript">
    function getValue(option) {
      var val=option.value;
    //alert(val);
     return val;
    }
  
  </script>

  </head>
  <body>
    <form method="get" name="form" action="#">
    <select id="choice" <?php echo 'onchange="getValue(this)"'; ?>>
    
      <option value="default" selected>Choose a Value</option>
      <option value="first">First Value</option>
      <option value="second">Second Value</option>
      <option value="third">Third Value</option>
    </select> 
    <?php echo $x;?>
  </form>  
    
  </body>
  </html>

