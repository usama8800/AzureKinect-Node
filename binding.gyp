{
    "targets": [
        {
            "target_name": "myaddon",
            "sources": [
                "cc_src/myaddon.cc",
                "cc_src/kinect.cc"
            ],
            "include_dirs": [
                "../cc_src",
            ],
            "libraries": [
                "-lk4a"
            ]
        }
    ]
}