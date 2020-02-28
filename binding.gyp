{
  "targets": [
    {
      "target_name": "azurekinect_node",
      "cflags!": [
        "-fno-exceptions"
      ],
      "cflags_cc!": [
        "-fno-exceptions"
      ],
      "sources": [
        "cc_src/addon.cc",
        "cc_src/kinect.cc"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "<(module_root_dir)/sdk/sensor-sdk/build/native/include",
        "<(module_root_dir)/sdk/body-tracking-sdk/build/native/include"
      ],
      "libraries": [
        "<(module_root_dir)/sdk/sensor-sdk/lib/native/amd64/release/k4a.lib",
        "<(module_root_dir)/sdk/sensor-sdk/lib/native/amd64/release/k4arecord.lib",
        "<(module_root_dir)/sdk/body-tracking-sdk/lib/native/amd64/release/k4abt.lib"
      ],
      "copies": [
        {
          "destination": "<(module_root_dir)/build/Release",
          "files": [
            "<(module_root_dir)/sdk/sensor-sdk/lib/native/amd64/release/depthengine_2_0.dll",
            "<(module_root_dir)/sdk/sensor-sdk/lib/native/amd64/release/k4a.dll",
            "<(module_root_dir)/sdk/sensor-sdk/lib/native/amd64/release/k4arecord.dll",
            "<(module_root_dir)/sdk/body-tracking-sdk/lib/native/amd64/release/k4abt.dll"
          ]
        }
      ],
      "defines": [
        "NAPI_DISABLE_CPP_EXCEPTIONS"
      ]
    }
  ]
}
