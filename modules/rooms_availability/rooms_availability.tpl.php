<div class="<?php print $variables['classes']?>">

<?php print render($title) ?>

<table>
  <tr>
    <td style="width:33%;text-align:left;">
        <?php print $backward_link ?>
    </td>
    <td style="width:33%;text-align:center">
        <?php print $current_link ?>      
    </td>
    <td style="width:33%;text-align:right">
      <?php print $forward_link ?>
    </td>
  </tr>
  <tr>
    <td><div id='calendar' class="month1"></div></td>
    <td><div id='calendar1' class="month2"></div></td>
    <td><div id='calendar2' class="month3"></div></td>
  </tr>

</table>

<div style="clear:both"></div>

<div class="availability-updates">

<?php print render($update_form_into); ?>

<?php print render($update_form); ?>
</div>

</div>