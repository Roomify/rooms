<div class="<?php print $variables['classes']?>">
<h1 style="text-transform:capitalize"><?php print $type ?> - <?php print $name ?> Availability View </h1>

<div id='date-backward'>
  <?php print $backward_link ?>
</div>
<div id='date-forward'>
  <?php print $forward_link ?>
</div>
<div style="clear:both"></div>

<div id='calendar' class="month1"></div>


<div id='calendar1' class="month2"></div>


<div id='calendar2' class="month3"></div>

<div style="clear:both"></div>


<h2>Update Room Availability</h2>

<p>Careful this will overwrite any existing bookings.</p>


<?php print render($variables['update_form']); ?>
</div>