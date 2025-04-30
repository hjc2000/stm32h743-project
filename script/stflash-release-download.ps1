. "script/prepare-for-release.ps1"

Push-Location $install_path
try
{
	if (Test-Path -Path "$install_path/bin/${project_name}.bin")
	{
		# bin 文件存在
		$is_newer = is-file-newer-than-reference.ps1 `
			-TargetFilePath "$install_path/bin/${project_name}.elf" `
			-ReferenceFilePath "$install_path/bin/${project_name}.bin"

		if (-not $is_newer)
		{
			# bin 文件存在，则只有 elf 比 bin 更新时才需要重新生成 bin 并下载。
			# 所以直接返回。
			Write-Host "不需要下载。"
			return
		}
	}

	$objcopy_cmd = @(
		"$env:cpp_lib_build_scripts_path/.toolchain/arm-none-eabi-14.2/bin/arm-none-eabi-objcopy"
		"-O binary"
		"$install_path/bin/${project_name}.elf"
		"$install_path/bin/${project_name}.bin"
	)
	$objcopy_cmd = $objcopy_cmd -join " "
	Invoke-Expression $objcopy_cmd

	st-flash --reset write "${install_path}/bin/${project_name}.bin" 0x8000000
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
