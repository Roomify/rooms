<?php //dpm($variables) ?>

<h1 style="text-transform:capitalize"><?php print $type ?> - <?php print $name ?> Availability View </h1>

<div id='calendar' class="month1"></div>


<div id='calendar1' class="month2"></div>


<div id='calendar2' class="month3"></div>

<div style="clear:both"></div>


<h2>Update Room Availability</h2>

<p>Careful this will overwrite any existing bookings.</p>



<?php print render($variables['update_form']); ?>