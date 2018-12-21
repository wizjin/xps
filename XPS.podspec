Pod::Spec.new do |spec|
  spec.name         = 'XPS'
  spec.version      = '0.1.0'
  spec.authors      = { 'wizjin' => 'wizjin@icloud.com' }
  spec.license      = { :type => 'MIT', :file => 'LICENSE' }
  spec.homepage     = 'https://github.com/wizjin/xps'
  spec.summary      = 'A light-weight X proxy system library.'
  spec.module_name  = 'XPS'
  spec.source       = { :git => 'https://github.com/wizjin/xps.git', :tag => "#{spec.version}" }
  spec.ios.deployment_target = '10.0'
  spec.osx.deployment_target = '10.11'
  spec.source_files          = 'libxps/**/*.{h,c}'
  spec.exclude_files         = 'tests/**/*.{h,c}'
  spec.public_header_files   = 'libxps/*.h'
end
