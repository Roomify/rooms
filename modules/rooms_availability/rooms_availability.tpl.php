<?php dpm($variables) ?>

<h1 style="text-transform:capitalize"><?php print $type ?> - <?php print $name ?> Availability View </h1>

<div id='calendar' style="float:left;width:400px;margin:20px 10px;"></div>


<div id='calendar1' style="float:left;width:400px;margin:20px 10px;"></div>


<div id='calendar2' style="float:left;width:400px;margin:20px 10px;"></div>

<div style="clear:both"></div>


<h2>Update Room Availability</h2>

<p>Careful this will overwrite any existing bookings.</p>



<?php print render($variables['update_form']); ?>