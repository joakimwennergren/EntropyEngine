    // vec3 ambient = vec3(0.5, 0.5, 0.5);
    // vec3 lighting = vec3(0.0, 80.0, 0.0);

    // mat4 camera = inInvView;
    // vec3 cameraSource = vec3(camera[3].x, camera[3].y, camera[3].z);

    // // Diffuse
    // vec3 normal = normalize(inNormal);
    // vec3 lightColor = vec3(1.0, 1.0, 1.0);
    // vec3 lightSource = vec3(1.0, 1.0, 0.0);
    // float diffuseStrength = max(0.0, dot(lightSource, normal));
    // vec3 diffuse = diffuseStrength * lightColor;

    // // Specular
    // vec3 viewSource = normalize(cameraSource);
    // vec3 reflectSource = normalize(reflect(-lightSource, normal));
    // float specularStrength = max(0.0, dot(viewSource, reflectSource));
    // specularStrength = pow(specularStrength, 32.0);
    // vec3 specular = specularStrength * lightColor;


    // float distance = length(vec3(0.0, 5.0, 0.0) - inWorldPos.xyz);

    // float attenuation = 1.0 / (1.0 + 0.001 * distance +
    //             0.008 * (distance * distance));

    // ambient  *= attenuation;
    // diffuse  *= attenuation;
    // specular *= attenuation;

    // lighting = ambient * 2.0 + diffuse * 0.3 + specular * 0.3;

    // vec4 sampled = texture(sampler2D(_texture, texSampler), inUV0) * vec4(lighting, 1.0);

    // float value = sin(time * 0.008);
    // vec4 color = inColor0;
    // //vec4 tinting_pulsed = vec4(tint.r, tint.g, tint.b, value);
    // //color = inColor0 * tint;

    // outColor = color * sampled;


    // //vec4 sampled = texture(sampler2D(_texture, texSampler), inUV0);