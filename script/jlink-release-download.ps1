. "script/release-build.ps1"

Push-Location $install_path
try
{
	$objcopy_cmd = @(
		"$env:cpp_lib_build_scripts_path/.toolchain/arm-none-eabi-14.2/bin/arm-none-eabi-objcopy"
		"-O binary"
		"$install_path/bin/${project_name}.elf"
		"$install_path/bin/${project_name}.bin"
	)

	$objcopy_cmd = $objcopy_cmd -join " "
	Invoke-Expression $objcopy_cmd

	$jflash_arg = @(
		"-openprj${workspace_path}/jflash-project.jflash",
		"-open${install_path}/bin/${project_name}.bin,0x8000000",
		"-auto",
		"-startapp",
		"-exit"
	)
	$jflash_arg = $jflash_arg -join " "
	$jflash_arg = $jflash_arg.Trim()
	Write-Host $jflash_arg

	Start-Process -FilePath "JFlash.exe" `
		-ArgumentList $jflash_arg `
		-WindowStyle Normal `
		-Wait

	if ($LASTEXITCODE)
	{
		try-remove-items.exe --paths "$install_path/bin/${project_name}.bin"
		throw "将 ${project_name}.bin 下载到单片机失败。"
	}

	Write-Host "将 ${project_name}.bin 下载到单片机成功。"
}
finally
{
	Pop-Location
}
