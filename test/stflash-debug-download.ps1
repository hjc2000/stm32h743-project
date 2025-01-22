# 项目参数
$project_name = "test"
$cmake_config = "gcc-debug"
$project_path = Split-Path -Path $MyInvocation.MyCommand.Definition -Parent
$workspace_path = Split-Path $project_path -Parent
$build_path = "$workspace_path/out/build/$cmake_config"
$install_path = "$workspace_path/out/install/$cmake_config"

# 开始操作
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

	$install_out_str = ninja install
	if (-not $install_out_str.Contains("Installing"))
	{
		Write-Host "已经是最新的，不需要下载。"
		return
	}
}
finally
{
	Pop-Location
}

Push-Location $install_path
try
{
	arm-none-eabi-objcopy -O binary `
		"$install_path/bin/${project_name}.elf" `
		"$install_path/bin/${project_name}.bin"

	st-flash --reset write "${install_path}/bin/${project_name}.bin" 0x8000000
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
