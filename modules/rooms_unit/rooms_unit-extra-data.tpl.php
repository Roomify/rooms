<?php

/**
 * @file
 * Example tpl file for theming a single unit-specific theme
 *
 * Available variables:
 * - $status: The variable to theme (while only show if you tick status)
 *
 * Helper variables:
 * - $rooms_room: The Room object the variables are derived from
 */
?>

<div class="rooms_unit-status">
  <?php print '<strong>Unit Sample Data:</strong> ' . $rooms_unit_sample_data = ($rooms_unit_sample_data) ? 'Switch On' : 'Switch Off' ?>
</div>
