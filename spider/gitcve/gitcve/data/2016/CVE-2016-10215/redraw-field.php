<?php
	$required = false;
	$label = "";
	$type = trim($_POST["type"]);
	$key = str_replace("form_builder_element_", "", $_POST["name"]);
	
	// Clean up prices
	if ($_POST["list"]["list"]) {
		foreach ($_POST["list"]["list"] as &$item) {
			if ($item["price"]) {
				$item["price"] = floatval(str_replace(array('$', ',', ' '), '', $item["price"]));
			} else {
				$item["price"] = "0";
			}
		}
		
		unset($item);
	}
	
	$data = $_POST;
?>
<input type="hidden" name="id[<?=$key?>]" value="<?=$_POST["id"]?>" />
<input type="hidden" name="type[<?=$key?>]" value="<?=$type?>" />
<input type="hidden" name="data[<?=$key?>]" value="<?=htmlspecialchars(json_encode($data))?>" />
<div class="form_builder_wrapper">
	<span class="icon"></span>
	<?php include EXTENSION_ROOT."modules/btx-form-builder/field-types/redraw/$type.php" ?>
</div>
<div class="form_builder_controls">
	<a href="#" class="icon_small icon_small_edit"></a>
	<a href="#" class="icon_small icon_small_delete"></a>
</div>