# This is untested
#This is a sample script to assist in revoking local admin for "Domain Users" from an endpoint. Workstations should be provisioned for individual users in most cases.
#This should hopefully help some orgs migrate from a situation where a random user can connect to a Domain Admin's system and steal their DA credentials. Thx Bloodhound.
#
# Get a list of user profile directories under C:\Users
$userProfiles = Get-ChildItem -Path 'C:\Users' | Where-Object { $_.PSIsContainer } | Select-Object -ExpandProperty Name

# Add each user to the local Administrators group
foreach ($profile in $userProfiles) {
    $localAdmin = [ADSI]"WinNT://./Administrators,group"
    $localAdmin.psbase.Invoke("Add",([ADSI]"WinNT://$profile").path)
}

# Remove the "Domain Users" group from the local Administrators group
$domainUsers = [ADSI]"WinNT://YourDomain/Domain Users,group"
$localAdmin.psbase.Invoke("Remove", $domainUsers.path)
