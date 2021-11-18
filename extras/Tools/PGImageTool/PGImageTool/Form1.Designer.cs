
namespace PGImageTool
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.SelectImagesButton = new System.Windows.Forms.Button();
            this.SrcFilesListView = new System.Windows.Forms.ListView();
            this.SelectOutputButton = new System.Windows.Forms.Button();
            this.OutputFileTextBox = new System.Windows.Forms.TextBox();
            this.GenerateButton = new System.Windows.Forms.Button();
            this.openFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.saveFileDialog = new System.Windows.Forms.SaveFileDialog();
            this.StatusLabel = new System.Windows.Forms.Label();
            this.ClearStatustimer = new System.Windows.Forms.Timer(this.components);
            this.SuspendLayout();
            // 
            // SelectImagesButton
            // 
            this.SelectImagesButton.Location = new System.Drawing.Point(13, 13);
            this.SelectImagesButton.Name = "SelectImagesButton";
            this.SelectImagesButton.Size = new System.Drawing.Size(199, 23);
            this.SelectImagesButton.TabIndex = 0;
            this.SelectImagesButton.Text = "Select Images";
            this.SelectImagesButton.UseVisualStyleBackColor = true;
            this.SelectImagesButton.Click += new System.EventHandler(this.SelectImagesButton_Click);
            // 
            // SrcFilesListView
            // 
            this.SrcFilesListView.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.SrcFilesListView.GridLines = true;
            this.SrcFilesListView.HideSelection = false;
            this.SrcFilesListView.Location = new System.Drawing.Point(13, 43);
            this.SrcFilesListView.Name = "SrcFilesListView";
            this.SrcFilesListView.Size = new System.Drawing.Size(540, 308);
            this.SrcFilesListView.TabIndex = 1;
            this.SrcFilesListView.UseCompatibleStateImageBehavior = false;
            this.SrcFilesListView.View = System.Windows.Forms.View.List;
            // 
            // SelectOutputButton
            // 
            this.SelectOutputButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.SelectOutputButton.Location = new System.Drawing.Point(13, 358);
            this.SelectOutputButton.Name = "SelectOutputButton";
            this.SelectOutputButton.Size = new System.Drawing.Size(199, 23);
            this.SelectOutputButton.TabIndex = 2;
            this.SelectOutputButton.Text = "Select Output File";
            this.SelectOutputButton.UseVisualStyleBackColor = true;
            this.SelectOutputButton.Click += new System.EventHandler(this.SelectOutputButton_Click);
            // 
            // OutputFileTextBox
            // 
            this.OutputFileTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.OutputFileTextBox.Location = new System.Drawing.Point(13, 388);
            this.OutputFileTextBox.Name = "OutputFileTextBox";
            this.OutputFileTextBox.Size = new System.Drawing.Size(540, 20);
            this.OutputFileTextBox.TabIndex = 3;
            // 
            // GenerateButton
            // 
            this.GenerateButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.GenerateButton.Location = new System.Drawing.Point(13, 415);
            this.GenerateButton.Name = "GenerateButton";
            this.GenerateButton.Size = new System.Drawing.Size(199, 23);
            this.GenerateButton.TabIndex = 4;
            this.GenerateButton.Text = "Generate";
            this.GenerateButton.UseVisualStyleBackColor = true;
            this.GenerateButton.Click += new System.EventHandler(this.GenerateButton_Click);
            // 
            // openFileDialog
            // 
            this.openFileDialog.FileName = "openFileDialog";
            this.openFileDialog.Filter = "PNG Files | *.png";
            this.openFileDialog.Multiselect = true;
            // 
            // saveFileDialog
            // 
            this.saveFileDialog.DefaultExt = "cpp";
            this.saveFileDialog.Filter = "CPP Files | *.cpp";
            // 
            // StatusLabel
            // 
            this.StatusLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.StatusLabel.AutoSize = true;
            this.StatusLabel.Location = new System.Drawing.Point(220, 420);
            this.StatusLabel.Name = "StatusLabel";
            this.StatusLabel.Size = new System.Drawing.Size(138, 13);
            this.StatusLabel.TabIndex = 5;
            this.StatusLabel.Text = "Generated files successfully";
            this.StatusLabel.Visible = false;
            // 
            // ClearStatustimer
            // 
            this.ClearStatustimer.Interval = 2000;
            this.ClearStatustimer.Tick += new System.EventHandler(this.ClearStatustimer_Tick);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(567, 450);
            this.Controls.Add(this.StatusLabel);
            this.Controls.Add(this.GenerateButton);
            this.Controls.Add(this.OutputFileTextBox);
            this.Controls.Add(this.SelectOutputButton);
            this.Controls.Add(this.SrcFilesListView);
            this.Controls.Add(this.SelectImagesButton);
            this.Name = "Form1";
            this.Text = "PGImageTool";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button SelectImagesButton;
        private System.Windows.Forms.ListView SrcFilesListView;
        private System.Windows.Forms.Button SelectOutputButton;
        private System.Windows.Forms.TextBox OutputFileTextBox;
        private System.Windows.Forms.Button GenerateButton;
        private System.Windows.Forms.OpenFileDialog openFileDialog;
        private System.Windows.Forms.SaveFileDialog saveFileDialog;
        private System.Windows.Forms.Label StatusLabel;
        private System.Windows.Forms.Timer ClearStatustimer;
    }
}

