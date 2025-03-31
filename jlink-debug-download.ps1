$ErrorActionPreference = "Stop"

# 项目参数
$project_name = "stm32h743-project"
$cmake_config = "arm-none-eabi-cortex-m7-debug"
$platform = "arm-none-eabi-cortex-m7"
$workspace_path = Split-Path -Path $MyInvocation.MyCommand.Definition -Parent
$build_path = "$workspace_path/jc_build"
$install_path = "$env:cpp_lib_build_scripts_path/${platform}/.libs/${project_name}"

# 开始操作
New-Item -Path $build_path -ItemType Directory -Force
Push-Location $build_path
try
{
	cmake -G "Ninja" $workspace_path `
		--preset "$cmake_config"

	if ($LASTEXITCODE)
	{
		throw "配置失败"
	}

	ninja -j12
	if ($LASTEXITCODE)
	{
		throw "编译失败"
	}

	ninja install
}
finally
{
	Pop-Location
}

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
			Write-Host "此 elf 已经是最新的，不需要下载。"
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

	$jflash_arg = @(
		"-openprj${workspace_path} / jflash-project.jflash",
		"-open${install_path}/bin/${project_name}.bin, 0x8000000",
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
		throw "将 ${project_name}.bin 下载到单片机失败。"
	}

	Write-Host "将 ${project_name}.bin 下载到单片机成功。"
}
finally
{
	Pop-Location
}
