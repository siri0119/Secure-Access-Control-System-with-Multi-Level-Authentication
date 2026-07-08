#include "lcd.h"
#include "uart.h"
#include "delay.h"

// Send, receive buffers and variables
u8 snd_buf[20], rec_buf[25], j=0, i=0, frame_ready=0;


// Send packet through UART
void send_packet(u8 *ptr,u8 n)
{
        // Send all bytes
        for(j=0;j<n;j++)
        {
                // Send one byte
                u0_Tx_byte(ptr[j]);
        }
}


// Capture fingerprint image
s8 Genimg(void)
{
        u16 timeout = 0;   // Timeout count

        // Clear frame flag
        frame_ready = 0;

        // Reset receive index
        i = 0;

        // Clear receive buffer
        for(j=0;j<25;j++)
        {
                rec_buf[j] = 0;
        }

        // Clear send buffer
        for(j=0;j<12;j++)
        {
                snd_buf[j] = 0;
        }

        // Set packet header
        snd_buf[0] = 0xEF;
        snd_buf[1] = 0x01;

        // Set device address
        snd_buf[2] = 0xFF;
        snd_buf[3] = 0xFF;
        snd_buf[4] = 0xFF;
        snd_buf[5] = 0xFF;

        // Set packet type
        snd_buf[6] = 0x01;

        // Set packet length
        snd_buf[7] = 0x00;
        snd_buf[8] = 0x03;

        // Set image capture command
        snd_buf[9] = 0x01;

        // Set checksum
        snd_buf[10] = 0x00;
        snd_buf[11] = 0x05;

        // Send packet
        send_packet(snd_buf,12);

        // Delay
        delay_ms(10);

        // Wait for response
        while(frame_ready == 0)
        {
                // Delay
                delay_ms(1);

                // Increase timeout count
                timeout++;

                // Check timeout
                if(timeout > 1000)
                {
                        // Return failure
                        return 1;
                }
        }

        // Check response
        if(rec_buf[9] == 0x00)
        {
                // Return success
                return 0;
        }
        else
        {
                // Return failure
                return 1;
        }
}


// Convert image to template buffer 1
s8 img2tp1(void)
{
        u16 timeout = 0;   // Timeout count

        // Clear frame flag
        frame_ready = 0;

        // Reset receive index
        i = 0;

        // Clear receive buffer
        for(j=0;j<25;j++)
        {
                rec_buf[j] = 0;
        }

        // Clear send buffer
        for(j=0;j<13;j++)
        {
                snd_buf[j] = 0;
        }

        // Set packet header
        snd_buf[0] = 0xEF;
        snd_buf[1] = 0x01;

        // Set device address
        snd_buf[2] = 0xFF;
        snd_buf[3] = 0xFF;
        snd_buf[4] = 0xFF;
        snd_buf[5] = 0xFF;

        // Set packet type
        snd_buf[6] = 0x01;

        // Set packet length
        snd_buf[7] = 0x00;
        snd_buf[8] = 0x04;

        // Set image conversion command
        snd_buf[9] = 0x02;

        // Select buffer 1
        snd_buf[10] = 0x01;

        // Set checksum
        snd_buf[11] = 0x00;
        snd_buf[12] = 0x08;

        // Clear frame flag
        frame_ready = 0;

        // Send packet
        send_packet(snd_buf,13);

        // Wait for response
        while(frame_ready == 0)
        {
                // Delay
                delay_ms(1);

                // Increase timeout count
                timeout++;

                // Check timeout
                if(timeout > 1000)
                {
                        // Return failure
                        return 1;
                }
        }

        // Check response
        if(rec_buf[9] == 0x00)
        {
                // Return success
                return 0;
        }
        else
        {
                // Return failure
                return 1;
        }
}


// Convert image to template buffer 2
s8 img2tp2(void)
{
        u16 timeout = 0;   // Timeout count

        // Clear frame flag
        frame_ready = 0;

        // Reset receive index
        i = 0;

        // Clear receive buffer
        for(j=0;j<25;j++)
        {
                rec_buf[j] = 0;
        }

        // Clear send buffer
        for(j=0;j<13;j++)
        {
                snd_buf[j] = 0;
        }

        // Set packet header
        snd_buf[0] = 0xEF;
        snd_buf[1] = 0x01;

        // Set device address
        snd_buf[2] = 0xFF;
        snd_buf[3] = 0xFF;
        snd_buf[4] = 0xFF;
        snd_buf[5] = 0xFF;

        // Set packet type
        snd_buf[6] = 0x01;

        // Set packet length
        snd_buf[7] = 0x00;
        snd_buf[8] = 0x04;

        // Set image conversion command
        snd_buf[9] = 0x02;

        // Select buffer 2
        snd_buf[10] = 0x02;

        // Set checksum
        snd_buf[11] = 0x00;
        snd_buf[12] = 0x09;

        // Send packet
        send_packet(snd_buf,13);

        // Wait for response
        while(frame_ready == 0)
        {
                // Delay
                delay_ms(1);

                // Increase timeout count
                timeout++;

                // Check timeout
                if(timeout > 1000)
                {
                        // Return failure
                        return 1;
                }
        }

        // Check response
        if(rec_buf[9] == 0x00)
        {
                // Return success
                return 0;
        }
        else
        {
                // Return failure
                return 1;
        }
}


// Generate fingerprint model
s8 regmodel(void)
{
        u16 timeout = 0;   // Timeout count

        // Clear frame flag
        frame_ready = 0;

        // Reset receive index
        i = 0;

        // Clear receive buffer
        for(j=0;j<25;j++)
        {
                rec_buf[j] = 0;
        }

        // Clear send buffer
        for(j=0;j<12;j++)
        {
                snd_buf[j] = 0;
        }

        // Set packet header
        snd_buf[0] = 0xEF;
        snd_buf[1] = 0x01;

        // Set device address
        snd_buf[2] = 0xFF;
        snd_buf[3] = 0xFF;
        snd_buf[4] = 0xFF;
        snd_buf[5] = 0xFF;

        // Set packet type
        snd_buf[6] = 0x01;

        // Set packet length
        snd_buf[7] = 0x00;
        snd_buf[8] = 0x03;

        // Set model command
        snd_buf[9] = 0x05;

        // Set checksum
        snd_buf[10] = 0x00;
        snd_buf[11] = 0x09;

        // Send packet
        send_packet(snd_buf,12);

        // Wait for response
        while(frame_ready == 0)
        {
                // Delay
                delay_ms(1);

                // Increase timeout count
                timeout++;

                // Check timeout
                if(timeout > 1000)
                {
                        // Return failure
                        return 1;
                }
        }

        // Check response
        if(rec_buf[9] == 0x00)
        {
                // Return success
                return 0;
        }
        else
        {
                // Return failure
                return 1;
        }
}


// Store fingerprint
s8 store(s8 id)
{
        u16 sum,timeout = 0;   // Checksum and timeout

        // Clear frame flag
        frame_ready = 0;

        // Reset receive index
        i = 0;

        // Clear receive buffer
        for(j=0;j<25;j++)
        {
                rec_buf[j] = 0;
        }

        // Clear send buffer
        for(j=0;j<15;j++)
        {
                snd_buf[j] = 0;
        }

        // Set packet header
        snd_buf[0] = 0xEF;
        snd_buf[1] = 0x01;

        // Set device address
        snd_buf[2] = 0xFF;
        snd_buf[3] = 0xFF;
        snd_buf[4] = 0xFF;
        snd_buf[5] = 0xFF;

        // Set packet type
        snd_buf[6] = 0x01;

        // Set packet length
        snd_buf[7] = 0x00;
        snd_buf[8] = 0x06;

        // Set store command
        snd_buf[9] = 0x06;

        // Select buffer 1
        snd_buf[10] = 0x01;

        // Set ID high byte
        snd_buf[11] = 0x00;

        // Set ID
        snd_buf[12] = id;

        // Calculate checksum
        sum = 0x01 + 0x00 + 0x06 + 0x06 + 0x01 + 0x00 + id;

        // Set checksum high byte
        snd_buf[13] = sum>>8;

        // Set checksum low byte
        snd_buf[14] = sum&0xff;

        // Send packet
        send_packet(snd_buf,15);

        // Wait for response
        while(frame_ready == 0)
        {
                // Delay
                delay_ms(1);

                // Increase timeout count
                timeout++;

                // Check timeout
                if(timeout > 1000)
                {
                        // Return failure
                        return 1;
                }
        }

        // Check response
        if(rec_buf[9] == 0x00)
        {
                // Return success
                return 0;
        }
        else
        {
                // Return failure
                return 1;
        }
}


// Load stored fingerprint
s8 load_char(s8 id)
{
        u16 sum,timeout = 0;   // Checksum and timeout

        // Clear frame flag
        frame_ready = 0;

        // Reset receive index
        i = 0;

        // Clear receive buffer
        for(j=0;j<25;j++)
        {
                rec_buf[j] = 0;
        }

        // Clear send buffer
        for(j=0;j<15;j++)
        {
                snd_buf[j] = 0;
        }

        // Set packet header
        snd_buf[0] = 0xEF;
        snd_buf[1] = 0x01;

        // Set device address
        snd_buf[2] = 0xFF;
        snd_buf[3] = 0xFF;
        snd_buf[4] = 0xFF;
        snd_buf[5] = 0xFF;

        // Set packet type
        snd_buf[6] = 0x01;

        // Set packet length
        snd_buf[7] = 0x00;
        snd_buf[8] = 0x06;

        // Set load command
        snd_buf[9] = 0x07;

        // Select buffer 2
        snd_buf[10] = 0x02;

        // Set ID high byte
        snd_buf[11] = 0x00;

        // Set ID
        snd_buf[12] = id;

        // Calculate checksum
        sum = 0x01 + 0x00 + 0x06 + 0x07 + 0x02 + 0x00 + id;

        // Set checksum high byte
        snd_buf[13] = (sum >> 8);

        // Set checksum low byte
        snd_buf[14] = (sum & 0xFF);

        // Send packet
        send_packet(snd_buf,15);

        // Wait for response
        while(frame_ready == 0)
        {
                // Delay
                delay_ms(1);

                // Increase timeout count
                timeout++;

                // Check timeout
                if(timeout > 1000)
                {
                        // Return failure
                        return 1;
                }
        }

        // Return response status
        return (rec_buf[9] == 0x00) ? 0 : 1;
}


// Match fingerprint templates
s8 match(void)
{
        u16 timeout = 0;   // Timeout count

        // Clear frame flag
        frame_ready = 0;

        // Reset receive index
        i = 0;

        // Clear receive buffer
        for(j=0;j<25;j++)
        {
                rec_buf[j] = 0;
        }

        // Clear send buffer
        for(j=0;j<12;j++)
        {
                snd_buf[j] = 0;
        }

        // Set packet header
        snd_buf[0] = 0xEF;
        snd_buf[1] = 0x01;

        // Set device address
        snd_buf[2] = 0xFF;
        snd_buf[3] = 0xFF;
        snd_buf[4] = 0xFF;
        snd_buf[5] = 0xFF;

        // Set packet type
        snd_buf[6] = 0x01;

        // Set packet length
        snd_buf[7] = 0x00;
        snd_buf[8] = 0x03;

        // Set match command
        snd_buf[9] = 0x03;

        // Set checksum
        snd_buf[10] = 0x00;
        snd_buf[11] = 0x07;

        // Send packet
        send_packet(snd_buf,12);

        // Wait for response
        while(frame_ready == 0)
        {
                // Delay
                delay_ms(1);

                // Increase timeout count
                timeout++;

                // Check timeout
                if(timeout > 100)
                {
                        // Return failure
                        return 1;
                }
        }

        // Return match status
        return (rec_buf[9] == 0x00) ? 0 : 1;
}


// Delete fingerprint
s8 delete(s8 id)
{
        u16 sum,timeout = 0;   // Checksum and timeout

        // Clear frame flag
        frame_ready = 0;

        // Reset receive index
        i = 0;

        // Clear receive buffer
        for(j=0;j<25;j++)
        {
                rec_buf[j] = 0;
        }

        // Clear send buffer
        for(j=0;j<15;j++)
        {
                snd_buf[j] = 0;
        }

        // Set packet header
        snd_buf[0] = 0xEF;
        snd_buf[1] = 0x01;

        // Set device address
        snd_buf[2] = 0xFF;
        snd_buf[3] = 0xFF;
        snd_buf[4] = 0xFF;
        snd_buf[5] = 0xFF;

        // Set packet type
        snd_buf[6] = 0x01;

        // Set packet length
        snd_buf[7] = 0x00;
        snd_buf[8] = 0x07;

        // Set delete command
        snd_buf[9] = 0x0C;

        // Set ID high byte
        snd_buf[10] = 0x00;

        // Set ID
        snd_buf[11] = id;

        // Set template count high byte
        snd_buf[12] = 0x00;

        // Set template count low byte
        snd_buf[13] = 0x01;

        // Calculate checksum
        sum = 0x01 + 0x00 + 0x07 + 0x0c + 0x00 + id + 0x00 + 0x01;

        // Set checksum high byte
        snd_buf[14] = (sum >> 8)&0xff;

        // Set checksum low byte
        snd_buf[15] = (sum & 0xFF);

        // Send packet
        send_packet(snd_buf,16);

        // Wait for response
        while(frame_ready == 0)
        {
                // Delay
                delay_ms(1);

                // Increase timeout count
                timeout++;

                // Check timeout
                if(timeout > 1000)
                {
                        // Return failure
                        return 1;
                }
        }

        // Return delete status
        return (rec_buf[9] == 0x00) ? 0 : 1;
}


// Enroll fingerprint
s8 enroll(s8 id)
{
        s8 ret,retry=0;   // Return value and retry count

        // Clear LCD
        cmd_lcd(0x01);

        // Display message
        str_lcd("PLACE FINGER ");

        // Display symbol
        char_lcd(2);

        // Wait for finger
        while(Genimg() != 0)
        {
                // Delay
                delay_ms(50);

                // Increase retry count
                retry++;

                // Check timeout
                if(retry > 100)
                {
                        // Clear LCD
                        cmd_lcd(0x01);

                        // Display timeout
                        str_lcd("Timeout");

                        // Delay
                        delay_ms(500);

                        // Return failure
                        return 1;
                }
        }

        // Capture fingerprint
        ret = Genimg();

        // Check capture status
        if(ret != 0)
        {
                // Clear LCD
                cmd_lcd(0x01);

                // Display failure message
                str_lcd("GenImg Fail");

                // Delay
                delay_ms(500);

                // Return failure
                return 1;
        }

        // Convert image to buffer 1
        ret = img2tp1();

        // Check conversion status
        if(ret != 0)
        {
                // Clear LCD
                cmd_lcd(0x01);

                // Display failure message
                str_lcd("Tz1 Fail");

                // Delay
                delay_ms(500);

                // Return failure
                return 1;
        }

        // Clear LCD
        cmd_lcd(0x01);

        // Ask to remove finger
        str_lcd("Remove Finger");

        // Wait until finger is removed
        while(Genimg() == 0)
        {
                // Delay
                delay_ms(50);
        }

        // Delay
        delay_ms(1000);

        // Clear LCD
        cmd_lcd(0x01);

        // Ask to place finger again
        str_lcd("PLACE AGAIN ");

        // Display symbol
        char_lcd(2);

        // Wait for finger
        while(Genimg() != 0)
        {
                // Delay
                delay_ms(50);
        }

        // Capture fingerprint again
        ret = Genimg();

        // Check capture status
        if(ret != 0)
        {
                // Clear LCD
                cmd_lcd(0x01);

                // Display failure message
                str_lcd("GenImg2 Fail");

                // Delay
                delay_ms(500);

                // Return failure
                return 1;
        }

        // Convert image to buffer 2
        ret = img2tp2();

        // Check conversion status
        if(ret != 0)
        {
                // Clear LCD
                cmd_lcd(0x01);

                // Display failure message
                str_lcd("Tz2 Fail");

                // Delay
                delay_ms(500);

                // Return failure
                return 1;
        }

        // Generate fingerprint model
        ret = regmodel();

        // Check model status
        if(ret != 0)
        {
                // Clear LCD
                cmd_lcd(0x01);

                // Display failure message
                str_lcd("Reg Fail");

                // Delay
                delay_ms(500);

                // Return failure
                return 1;
        }

        // Store fingerprint
        ret = store(id);

        // Check store status
        if(ret != 0)
        {
                // Clear LCD
                cmd_lcd(0x01);

                // Display failure message
                str_lcd("Store Fail");

                // Delay
                delay_ms(500);

                // Return failure
                return 1;
        }

        // Clear LCD
        cmd_lcd(0x01);

        // Display success message
        str_lcd("ENROLL DONE ");

        // Display symbol
        char_lcd(1);

        // Delay
        delay_ms(500);

        // Return success
        return 0;
}


// Verify fingerprint
s8 verify(s8 id)
{
        s8 ret;   // Return value

        // Clear LCD
        cmd_lcd(0x01);

        // Display message
        str_lcd("PLACE FINGER ");

        // Display symbol
        char_lcd(2);

        // Wait for finger
        while(Genimg() != 0)
        {
                // Delay
                delay_ms(50);
        }

        // Capture fingerprint
        ret = Genimg();

        // Check capture status
        if(ret != 0)
        {
                // Clear LCD
                cmd_lcd(0x01);

                // Display failure message
                str_lcd("Gen Fail");

                // Delay
                delay_ms(500);

                // Return failure
                return 1;
        }

        // Convert image to buffer 1
        ret = img2tp1();

        // Check conversion status
        if(ret != 0)
        {
                // Clear LCD
                cmd_lcd(0x01);

                // Display failure message
                str_lcd("Tz Fail");

                // Delay
                delay_ms(500);

                // Return failure
                return 1;
        }

        // Load stored fingerprint
        ret = load_char(id);

        // Check load status
        if(ret != 0)
        {
                // Clear LCD
                cmd_lcd(0x01);

                // Display failure message
                str_lcd("Load Fail");

                // Delay
                delay_ms(500);

                // Return failure
                return 1;
        }

        // Match fingerprints
        ret = match();

        // Check match status
        if(ret == 0)
        {
                // Return success
                return 0;
        }
        else
        {
                // Return failure
                return 1;
        }
}
