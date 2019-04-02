# Terraform Configuration for ThingPolicy
#
# Install terraform 
#     brew install terraform
# Initialize
#     terraform init
# Use apply to create the resources
#     terraform apply


provider "aws" {
  region     = "us-east-1"
}

data "aws_caller_identity" "current" {}
data "aws_region" "current" {}

resource "aws_iot_policy" "thingpolicy" {
  name = "ThingPolicy"

  policy = <<EOF
{
    "Version": "2012-10-17",
    "Statement": [
        {
            "Effect": "Allow",
            "Action": "iot:Connect",
            "Resource": "arn:aws:iot:${data.aws_region.current.name}:${data.aws_caller_identity.current.account_id}:client/$${iot:Certificate.Subject.CommonName}"
        },
        {
            "Effect": "Allow",
            "Action": [
                "iot:Publish", 
                "iot:Receive"
            ],
            "Resource": "arn:aws:iot:${data.aws_region.current.name}:${data.aws_caller_identity.current.account_id}:topic/things/$${iot:ClientId}/*"
        },
        {
            "Effect": "Allow",
            "Action": "iot:Subscribe",
            "Resource": "arn:aws:iot:${data.aws_region.current.name}:${data.aws_caller_identity.current.account_id}:topicfilter/things/$${iot:ClientId}/*"
        }
    ]
}
EOF
}