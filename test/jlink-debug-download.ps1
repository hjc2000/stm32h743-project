# 项目参数
$project_name = "test"
$project_path = Split-Path -Path $MyInvocation.MyCommand.Definition -Parent
$workspace_path = Split-Path $project_path -Parent
$build_path = "$workspace_path/out/build/gcc-debug"
$install_path = "$workspace_path/out/install/gcc-debug"

# 开始操作
Push-Location $build_path
try
{
	ninja install
}
finally
{
	Pop-Location
}

Push-Location $install_path
try
{
	arm-none-eabi-objcopy -O binary "$install_path/bin/${project_name}.elf" "$install_path/bin/${project_name}.bin"

	Start-Process -FilePath "JFlash.exe" `
		-ArgumentList "-openprj${workspace_path}/jflash-project.jflash -open${install_path}/bin/${project_name}.bin,0x8000000 -auto -exit" `
		-WindowStyle Minimized

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
