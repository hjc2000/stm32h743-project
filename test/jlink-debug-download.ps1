# 项目参数
$project_name = "test"
$cmake_config = "gcc-debug"
$project_path = Split-Path -Path $MyInvocation.MyCommand.Definition -Parent
$workspace_path = Split-Path $project_path -Parent
$build_path = "$workspace_path/out/build/$cmake_config"
$install_path = "$workspace_path/out/install/$cmake_config"

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

	$build_out_str = ninja -j12
	Write-Host $build_out_str
	if ($build_out_str.Contains("ninja: no work to do"))
	{
		Write-Host "已经是最新的，不需要下载。"
		return
	}

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
	arm-none-eabi-objcopy -O binary `
		"$install_path/bin/${project_name}.elf" `
		"$install_path/bin/${project_name}.bin"

	$jflash_arg_array = @(
		"-openprj${workspace_path}/jflash-project.jflash",
		"-open${install_path}/bin/${project_name}.bin,0x8000000",
		"-auto",
		"-startapp",
		"-exit"
	)

	$jflash_arg = $jflash_arg_array -join " "
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
